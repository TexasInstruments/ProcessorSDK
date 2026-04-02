# Copyright (C) 2023 Texas Insruments, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

ifeq ($(BUILD_DEBUG),1)
$(info TI_TOOLS_ROOT=$(TI_TOOLS_ROOT))
$(info TIARMCGT_LLVM_ROOT=$(TIARMCGT_LLVM_ROOT))
endif

# DEP_PROJECTS does not need to be set as the dependencies are contained in the build.

SYSLDIRS :=
SYSDEFS  :=
SYSIDIRS :=

SYSIDIRS += $(HOST_ROOT)

ifeq ($(TARGET_PLATFORM), $(filter $(TARGET_PLATFORM), J721E J721S2 J784S4 J742S2 AM62A J722S))
    SYSDEFS +=
    ifeq ($(TARGET_FAMILY),ARM)
        ifeq ($(TARGET_CPU),$(filter $(TARGET_CPU), A72 A53))
            ifeq ($(TARGET_OS),QNX)
                SYSIDIRS += $(PDK_QNX_PATH)/packages
                SYSIDIRS += $(PDK_QNX_PATH)/packages/ti/osal
                SYSIDIRS += $(PDK_QNX_PATH)/packages/ti/drv
                SYSIDIRS += $(GCC_QNX_ARM_ROOT)/../usr/include
                SYSLDIRS += $(GCC_QNX_ARM_ROOT)/../usr/lib
                SYSDEFS  += QNX_OS
                SYSDEFS  += BUILD_MPU1_0
                SYSDEFS  += $(TARGET_PLATFORM)
            else
                SYSIDIRS += $(LINUX_FS_PATH)/usr/include
                SYSLDIRS += $(LINUX_FS_PATH)/usr/lib
            endif
            INSTALL_LIB := /usr/lib
            INSTALL_BIN := /usr/bin
            INSTALL_INC := /usr/include
        else
            SYSIDIRS += $(TIARMCGT_LLVM_ROOT)/include
            SYSLDIRS += $(TIARMCGT_LLVM_ROOT)/lib
        endif
    else ifeq ($(TARGET_FAMILY),DSP)
        ifeq ($(TARGET_CPU),C66)
            SYSIDIRS += $(CGT6X_ROOT)/include
            SYSLDIRS += $(CGT6X_ROOT)/lib
        else
            SYSDEFS  += C7X_FAMILY
            SYSIDIRS += $(CGT7X_ROOT)/include
            SYSLDIRS += $(CGT7X_ROOT)/lib
        endif
    endif

    ifeq ($(TARGET_OS), $(filter $(TARGET_OS), FREERTOS SAFERTOS THREADX))
        ifeq ($(RTOS_SDK),pdk)
            SYSIDIRS += $(PDK_PATH)/packages
            SYSIDIRS += $(PDK_PATH)/packages/ti/osal
            SYSIDIRS += $(PDK_PATH)/packages/ti/drv
        else
            SYSIDIRS += $(MCU_PLUS_SDK_PATH)/source
            SYSIDIRS += $(MCU_PLUS_SDK_PATH)/source/drivers
            SYSIDIRS += $(MCU_PLUS_SDK_PATH)/source/kernel/dpl
            SYSDEFS  += MCU_PLUS_SDK
        endif
    endif
endif
