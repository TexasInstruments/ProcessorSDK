
include tools_path.mak
include build_flags.mak

# Project specific build defs (don't change across different combos):
BUILD_DEFS :=

BUILD_DEFS += $(SOC_DEF)
BUILD_DEFS += $(VPAC_VERSION)

ifeq ($(SOC), am62a)
ifeq ($(BUILD_QNX_MPU), yes)
BUILD_DEFS += BUILD_QNX
endif
endif

ifeq ($(LDRA_COVERAGE_ENABLED_IMAGING), yes)
BUILD_DEFS += LDRA_COVERAGE
endif

ifeq ($(BUILD_IMAGING_OPENACC)$(VPAC_VERSION), yesVPAC3)
    BUILD_DEFS += ACC_VHWA
endif

ifeq ($(SOC), am62a)
BUILD_DEFS += $(TISDK_IMAGE)
endif

DIRECTORIES := .

TARGET_COMBOS :=

ifeq ($(BUILD_TARGET_MODE),yes)
  ifeq ($(PROFILE), $(filter $(PROFILE), debug all))
    TARGET_COMBOS += $(TARGET_SOC):$(RTOS):R5F:1:debug:TIARMCGT_LLVM
    ifeq ($(TARGET_SOC),J721E)
        TARGET_COMBOS += $(TARGET_SOC):$(RTOS):C66:1:debug:CGT6X
    else
        TARGET_COMBOS += $(TARGET_SOC):$(RTOS):$(C7X_TARGET):1:debug:CGT7X
    endif
    ifeq ($(BUILD_LINUX_MPU),yes)
      TARGET_COMBOS += $(TARGET_SOC):LINUX:$(MPU_CPU):1:debug:GCC_LINUX_ARM
    endif
    ifeq ($(BUILD_QNX_MPU),yes)
      TARGET_COMBOS += $(TARGET_SOC):QNX:$(MPU_CPU):1:debug:GCC_QNX_ARM
    endif
  endif

  ifeq ($(PROFILE), $(filter $(PROFILE), release all))
    TARGET_COMBOS += $(TARGET_SOC):$(RTOS):R5F:1:release:TIARMCGT_LLVM
    ifeq ($(TARGET_SOC),J721E)
        TARGET_COMBOS += $(TARGET_SOC):$(RTOS):C66:1:release:CGT6X
    else
        TARGET_COMBOS += $(TARGET_SOC):$(RTOS):$(C7X_TARGET):1:release:CGT7X
    endif
    ifeq ($(BUILD_LINUX_MPU),yes)
      TARGET_COMBOS += $(TARGET_SOC):LINUX:$(MPU_CPU):1:release:GCC_LINUX_ARM
    endif
    ifeq ($(BUILD_QNX_MPU),yes)
      TARGET_COMBOS += $(TARGET_SOC):QNX:$(MPU_CPU):1:release:GCC_QNX_ARM
    endif
  endif

endif

ifeq ($(BUILD_EMULATION_MODE),yes)
  ifeq ($(OS),Windows_NT)
    ifeq ($(PROFILE), $(filter $(PROFILE), debug all))
        ifeq ($(BUILD_EMULATION_ARCH), $(filter $(BUILD_EMULATION_ARCH), x86_64 all))
            TARGET_COMBOS += PC:WINDOWS:x86_64:1:debug:GCC_WINDOWS
        endif
        ifeq ($(BUILD_EMULATION_ARCH), $(filter $(BUILD_EMULATION_ARCH), X86 all))
            TARGET_COMBOS += PC:WINDOWS:X86:1:debug:GCC_WINDOWS
        endif
    endif

    ifeq ($(PROFILE), $(filter $(PROFILE), release all))
        ifeq ($(BUILD_EMULATION_ARCH), $(filter $(BUILD_EMULATION_ARCH), x86_64 all))
            TARGET_COMBOS += PC:WINDOWS:x86_64:1:release:GCC_WINDOWS
        endif
        ifeq ($(BUILD_EMULATION_ARCH), $(filter $(BUILD_EMULATION_ARCH), X86 all))
            TARGET_COMBOS += PC:WINDOWS:X86:1:release:GCC_WINDOWS
        endif
    endif
  else
    ifeq ($(PROFILE), $(filter $(PROFILE), debug all))
        ifeq ($(BUILD_EMULATION_ARCH), $(filter $(BUILD_EMULATION_ARCH), x86_64 all))
            ifeq ($(BUILD_IMAGING_OPENACC)$(VPAC_VERSION), yesVPAC3)
                TARGET_COMBOS += PC:LINUX:x86_64:1:debug:NVCPP_LINUX
            else
                TARGET_COMBOS += PC:LINUX:x86_64:1:debug:GCC_LINUX
            endif
        endif
        ifeq ($(BUILD_EMULATION_ARCH), $(filter $(BUILD_EMULATION_ARCH), X86 all))
            ifeq ($(BUILD_IMAGING_OPENACC)$(VPAC_VERSION), yesVPAC3)
                TARGET_COMBOS += PC:LINUX:X86:1:debug:NVCPP_LINUX
            else
                TARGET_COMBOS += PC:LINUX:X86:1:debug:GCC_LINUX
            endif
        endif
    endif

    ifeq ($(PROFILE), $(filter $(PROFILE), release all))
        ifeq ($(BUILD_EMULATION_ARCH), $(filter $(BUILD_EMULATION_ARCH), x86_64 all))
            ifeq ($(BUILD_IMAGING_OPENACC)$(VPAC_VERSION), yesVPAC3)
                TARGET_COMBOS += PC:LINUX:x86_64:1:release:NVCPP_LINUX
            else
                TARGET_COMBOS += PC:LINUX:x86_64:1:release:GCC_LINUX
            endif
        endif
        ifeq ($(BUILD_EMULATION_ARCH), $(filter $(BUILD_EMULATION_ARCH), X86 all))
            ifeq ($(BUILD_IMAGING_OPENACC)$(VPAC_VERSION), yesVPAC3)
                TARGET_COMBOS += PC:LINUX:X86:1:release:NVCPP_LINUX
            else
                TARGET_COMBOS += PC:LINUX:X86:1:release:GCC_LINUX
            endif
        endif
    endif
  endif
endif

CONCERTO_ROOT ?= concerto
BUILD_MULTI_PROJECT := 1
BUILD_TARGET := target.mak
BUILD_PLATFORM :=

include $(CONCERTO_ROOT)/rules.mak
