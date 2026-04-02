PSDK_PATH ?= $(abspath ../../../)
ALGBASE_PATH ?= $(PSDK_PATH)/tiadalg
KERNEL_NAME ?= $(shell basename $(abspath ./../))
TARGET_CPU ?= C71
test_build ?= 1 

ifeq ($(TARGET_CPU),C66)
	LINKER_CMD_FILE := $(ALGBASE_PATH)/common/linker_dsp_c66x.cmd
else
	LINKER_CMD_FILE := $(ALGBASE_PATH)/$(KERNEL_NAME)/test/c7x_linker_freertos.cmd
endif

include $(ALGBASE_PATH)/makerules/makefile

