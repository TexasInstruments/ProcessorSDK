
ifeq ($(BUILD_DEBUG),1)
$(info TI_TOOLS_ROOT=$(TI_TOOLS_ROOT))
$(info TIARMCGT_LLVM_ROOT=$(TIARMCGT_LLVM_ROOT))
endif

# DEP_PROJECTS does not need to be set as the dependencies are contained in the build.

SYSIDIRS := $(TIOVX_PATH)/include
SYSIDIRS += $(TIOVX_PATH)/kernels/include
SYSIDIRS += $(HOST_ROOT)/kernels/include

SYSLDIRS :=

SYSDEFS  :=

ifeq ($(TARGET_PLATFORM), $(filter $(TARGET_PLATFORM), J721E J721S2 J784S4 J742S2 AM62A))
    SYSDEFS +=
    ifeq ($(TARGET_FAMILY),ARM)
        ifeq ($(TARGET_CPU),$(filter $(TARGET_CPU), A72 A53))
            ifeq ($(TARGET_OS),QNX)
                SYSIDIRS += $(PDK_QNX_PATH)/packages
                SYSIDIRS += $(PDK_QNX_PATH)/packages/ti/osal
                SYSIDIRS += $(PDK_QNX_PATH)/packages/ti/drv
                SYSIDIRS += $(GCC_QNX_ARM_ROOT)/../usr/include
                SYSLDIRS += $(GCC_QNX_ARM_ROOT)/../usr/lib
            endif
        endif
    endif
endif
