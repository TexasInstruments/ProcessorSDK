#
# This file is the makefile for building DM Unit Test application.
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

APP_NAME = dm_unit_testapp_$(BUILD_OS_TYPE)

SRCDIR = . ./src ./soc/$(SOC) ./tests/device ./tests/clock
INCDIR = . ./include

# List all the components required by the application
COMP_LIST_COMMON  = $(PDK_COMMON_FREERTOS_COMP)

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk sciclient board uart freertos

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON = dm_ut_main.c dm_ut_devices.c dm_ut_testparser.c dm_ut_devices_list.c dm_ut_clocks.c dm_ut_clocks_list.c

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CFLAGS_OS_DEFINES)
PACKAGE_SRCS_COMMON = .

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
