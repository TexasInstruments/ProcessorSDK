# Makefile for FATFS unit test app
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
export DISABLE_RECURSE_DEPS

#Name of the directory created under packages/ti/binary/
APP_NAME = FATFS_Console_TestApp_$(BUILD_OS_TYPE)

ifeq ($(BUILD_OS_TYPE),freertos)
	CFLAGS_OS_DEFINES = -DFREERTOS
	# List all the external components/interfaces, whose interface header files
	# need to be included for this component
	INCLUDE_EXTERNAL_INTERFACES = freertos pdk
	COMP_LIST_COMMON = $(PDK_COMMON_FREERTOS_COMP)
endif

ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
SRCDIR = ./src ../
INCDIR = ./src
# Common source files across all platforms and cores
SRCS_COMMON += main_console.c FATFS_log.c fs_shell_app_utils.c
endif

# List all the components required by the application
COMP_LIST_COMMON += mmcsd fatfs_indp

CFLAGS_FATFS_UT =


PACKAGE_SRCS_COMMON = ../
CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CFLAGS_FATFS_UT) $(CFLAGS_OS_DEFINES)

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
