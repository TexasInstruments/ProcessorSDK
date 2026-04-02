ALGBASE_PATH ?= $(abspath ../)
PSDK_PATH ?= $(abspath ../../)
TARGET_CPU ?= C66
KERNEL_NAME ?= $(shell basename $(abspath ./))

ifeq ($(TARGET_CPU),C66)
LINKER_CMD_FILE = $(ALGBASE_PATH)/common/linker_dsp_c66x_ddr_heap.cmd
else
LINKER_CMD_FILE = $(ALGBASE_PATH)/common/linker_dsp_c7x_0.cmd
endif

include $(ALGBASE_PATH)/makerules/makefile

debug_print:
	echo $(ALGBASE_PATH)
	echo $(PSDK_PATH)
	echo $(TARGET_CPU)
	echo $(KERNEL_NAME)

