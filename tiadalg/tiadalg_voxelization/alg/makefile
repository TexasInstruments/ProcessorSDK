ALGBASE_PATH ?= $(abspath ./../../)
PSDK_PATH ?= $(abspath ./../../../)
KERNEL_NAME ?= $(shell basename $(abspath ./../))
TARGET_CPU ?= C71
test_build ?= 0

include $(ALGBASE_PATH)/makerules/makefile

debug_print:
	echo $(ALGBASE_PATH)
	echo $(PSDK_PATH)
	echo $(KERNEL_NAME)
	echo $(TARGET_CPU)				
	