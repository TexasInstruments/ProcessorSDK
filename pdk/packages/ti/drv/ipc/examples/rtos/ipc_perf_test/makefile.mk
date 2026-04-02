#
# This file is the makefile for building IPC performance example app
#
SRCDIR = . ../../common/src
INCDIR =

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

# List all the components required by the application
COMP_LIST_COMMON = ipc

SRCS_COMMON += main.c
ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
  ifeq ($(CORE),mcu1_0)
    COMP_LIST_COMMON += sciserver_tirtos
  endif
endif

ifeq ($(BUILD_OS_TYPE), freertos)
  INCLUDE_EXTERNAL_INTERFACES += freertos
  CFLAGS_LOCAL_COMMON = -DFREERTOS
  COMP_LIST_COMMON +=  $(PDK_COMMON_FREERTOS_COMP)
  SRCS_COMMON += ipc_trace.c
  ifeq ($(ISA), r5f)
	  SRCS_COMMON += r5f_mpu_$(SOC)_default.c
  endif
  ifeq ($(ISA), c66)
    INCDIR += ../../common/$(SOC)/$(BUILD_OS_TYPE)/
    SRCS_COMMON += c66_cache_mar.c
  endif
  EXTERNAL_LNKCMD_FILE_LOCAL = $(PDK_INSTALL_PATH)/ti/drv/ipc/examples/common/$(SOC)/$(BUILD_OS_TYPE)/linker_$(ISA)_$(CORE)_$(BUILD_OS_TYPE).lds
  APPEND_LNKCMD_FILE += $(PDK_INSTALL_PATH)/ti/drv/ipc/examples/common/$(SOC)/$(BUILD_OS_TYPE)/memory_map_ddr.cmd
endif

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON = . ../../common/src ../../common/$(SOC)
SRCS_COMMON += ipc_perf_test.c ipc_apputils.c ipc_test_defs.c

CFLAGS_LOCAL_COMMON += $(PDK_CFLAGS)

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
