PSDK_PATH ?= $(abspath ..)
PSDK_BUILDER_PATH ?= $(PSDK_PATH)/sdk_builder

include $(PSDK_BUILDER_PATH)/tools_path.mak

# Edit below file to change default build options
include $(PSDK_BUILDER_PATH)/build_flags.mak

# Project specific build defs (don't change across different combos):

BUILD_DEFS += $(SOC_DEF)

ifeq ($(SOC), am62a)
BUILD_DEFS += $(TISDK_IMAGE)
ifeq ($(BUILD_QNX_MPU),yes)
BUILD_DEFS+=QNX_MPU
endif
endif

DIRECTORIES := utils

ifeq ($(or $(BUILD_MPU),$(BUILD_R5F),$(BUILD_C7X),$(BUILD_C66)),)
	BUILD_MPU = 1
	BUILD_R5F = 1
	BUILD_C7X = 1
	BUILD_C66 = 1
endif

TARGET_COMBOS :=

ifeq ($(BUILD_EMULATION_MODE),yes)
	ifeq ($(PROFILE), $(filter $(PROFILE), debug all))
		TARGET_COMBOS += PC:LINUX:x86_64:1:debug:GCC_LINUX
	endif

	ifeq ($(PROFILE), $(filter $(PROFILE), release all))
		TARGET_COMBOS += PC:LINUX:x86_64:1:release:GCC_LINUX
	endif
endif

ifeq ($(BUILD_TARGET_MODE),yes)
	ifeq ($(PROFILE), $(filter $(PROFILE), debug all))
		ifeq ($(BUILD_MPU), 1)
			ifeq ($(BUILD_LINUX_MPU),yes)
				TARGET_COMBOS += $(TARGET_SOC):LINUX:$(MPU_CPU):1:debug:GCC_LINUX_ARM
			endif
			ifeq ($(BUILD_QNX_MPU),yes)
				TARGET_COMBOS += $(TARGET_SOC):QNX:$(MPU_CPU):1:debug:GCC_QNX_ARM
			endif
		endif
		ifeq ($(BUILD_R5F), 1)
			TARGET_COMBOS += $(TARGET_SOC):$(RTOS):R5F:1:debug:TIARMCGT_LLVM
		endif
		ifeq ($(BUILD_C7X), 1)
			TARGET_COMBOS += $(TARGET_SOC):$(RTOS):$(C7X_TARGET):1:debug:CGT7X
		endif
		ifeq ($(BUILD_C66), 1)
			ifeq ($(TARGET_SOC),J721E)
				TARGET_COMBOS += $(TARGET_SOC):$(RTOS):C66:1:debug:CGT6X
			endif
		endif
	endif

	ifeq ($(PROFILE), $(filter $(PROFILE), release all))
		ifeq ($(BUILD_MPU), 1)
			ifeq ($(BUILD_LINUX_MPU),yes)
				TARGET_COMBOS += $(TARGET_SOC):LINUX:$(MPU_CPU):1:release:GCC_LINUX_ARM
			endif
			ifeq ($(BUILD_QNX_MPU),yes)
				TARGET_COMBOS += $(TARGET_SOC):QNX:$(MPU_CPU):1:release:GCC_QNX_ARM
			endif
		endif
		ifeq ($(BUILD_R5F), 1)
			TARGET_COMBOS += $(TARGET_SOC):$(RTOS):R5F:1:release:TIARMCGT_LLVM
		endif
		ifeq ($(BUILD_C7X), 1)
			TARGET_COMBOS += $(TARGET_SOC):$(RTOS):$(C7X_TARGET):1:release:CGT7X
		endif
		ifeq ($(BUILD_C66), 1)
			ifeq ($(TARGET_SOC),J721E)
				TARGET_COMBOS += $(TARGET_SOC):$(RTOS):C66:1:release:CGT6X
			endif
		endif
	endif
endif

ERROR_FLAG = 0

# info statements need to run as Makefile statements, not commands. No indent tab is preceded
ifeq ($(LDRA_COVERAGE_ENABLED), yes)
	ifeq ($(INSTRUMENT_CORE), )
$(info ERROR - INSTRUMENT_CORE flag missing (A72 A53 R5F C71 C7120 C7504 C7524 C66))
		ERROR_FLAG = 1
	endif
else ifeq ($(INSTRUMENT_UTILS), yes)
$(info ERROR - LDRA_COVERAGE_ENABLED flag missing)
	ERROR_FLAG = 1
else ifneq ($(INSTRUMENT_CORE), )
$(info ERROR - LDRA_COVERAGE_ENABLED flag missing)
	ERROR_FLAG = 1
endif

ifeq ($(ERROR_FLAG), 1)
	TARGET_COMBOS =
endif


CONCERTO_ROOT ?= $(PSDK_BUILDER_PATH)/concerto
BUILD_MULTI_PROJECT := 1
BUILD_TARGET := $(PSDK_BUILDER_PATH)/target.mak
BUILD_PLATFORM :=

include $(CONCERTO_ROOT)/rules.mak

# Project specific rules

.PHONY: all
all:

clean_r5f:
	-rm -rf $(APP_UTILS_PATH)/out/$(TARGET_SOC)/R5F/

clean_c7x:
	-rm -rf $(APP_UTILS_PATH)/out/$(TARGET_SOC)/C7*/

clean_mpu:
	-rm -rf $(APP_UTILS_PATH)/out/$(TARGET_SOC)/A*/

clean_c66:
	-rm -rf $(APP_UTILS_PATH)/out/$(TARGET_SOC)/C66/
