#
# This is the makefile for building IPC graceful shutdown test
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

APP_NAME = ipc_graceful_shutdown_test_$(BUILD_OS_TYPE)

#Path for including common files
IPC_COMMON_PATH = ../../common

SRCDIR = . $(IPC_COMMON_PATH)/src
INCDIR = .

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

SOC_DIR=$(SOC)
ifeq ($(SOC), j742s2)
  SOC_DIR=j784s4
endif

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON = .
PACKAGE_SRCS_COMMON += $(IPC_COMMON_PATH)/src $(IPC_COMMON_PATH)/$(SOC_DIR)

ifeq ($(BUILD_OS_TYPE), freertos)
	COMP_LIST_COMMON =  $(PDK_COMMON_FREERTOS_COMP)
	COMP_LIST_COMMON += ipc
	ifeq ($(CORE),mcu1_0)
		COMP_LIST_COMMON += sciserver_tirtos
	endif
	CFLAGS_LOCAL_COMMON += -DFREERTOS
	INCLUDE_EXTERNAL_INTERFACES += freertos
	ifeq ($(ISA), r5f)
		SRCS_COMMON = r5f_mpu_$(SOC_DIR)_default.c
	endif
	EXTERNAL_LNKCMD_FILE_LOCAL = $(PDK_INSTALL_PATH)/ti/drv/ipc/examples/common/$(SOC_DIR)/$(BUILD_OS_TYPE)/linker_$(ISA)_$(CORE)_$(BUILD_OS_TYPE).lds
	APPEND_LNKCMD_FILE += $(PDK_INSTALL_PATH)/ti/drv/ipc/examples/common/$(SOC_DIR)/$(BUILD_OS_TYPE)/memory_map_ddr.cmd
endif

SRCS_COMMON += ipc_graceful_shutdown_test_config.c ipc_graceful_shutdown_main.c ipc_graceful_shutdown_test.c
SRCS_COMMON += ipc_trace.c

CFLAGS_LOCAL_COMMON += $(PDK_CFLAGS) -DENABLE_UART_PRINT

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