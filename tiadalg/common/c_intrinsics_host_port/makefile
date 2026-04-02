ALGBASE_PATH ?= $(abspath ../../)
PSDK_PATH ?= $(abspath ../../../)
TARGET_CPU ?= C66
TARGET_PLATFORM ?= PC

KERNEL_NAME ?= c6xsim
USERSUBDIRS = $(ALGBASE_PATH)/common/c_intrinsics_host_port/c6xsim

include $(ALGBASE_PATH)/makerules/makefile

debug_print:
	echo $(ALGBASE_PATH)
	echo $(PSDK_PATH)
	echo $(TARGET_CPU)
	echo $(KERNEL_NAME)
	echo $(USERSUBDIRS)


