################################################################################
# Configure SafeRTOS Package paths for various SOC/ISA
################################################################################

include $(PDK_INSTALL_PATH)/ti/build/safertos_version.mk
include $(PDK_INSTALL_PATH)/ti/build/makerules/platform.mk

ifeq ($(ISA),$(filter $(ISA), r5f c66 c7x))
  ifneq ($(SAFERTOS_$(SOC)_$(ISA)_INSTALL_PATH),)
    export SAFERTOS_KERNEL_INSTALL_PATH         = $(SAFERTOS_$(SOC)_$(ISA)_INSTALL_PATH)
    export SAFERTOS_KERNEL_INSTALL_PATH_$(ISA) ?= $(SAFERTOS_$(SOC)_$(ISA)_INSTALL_PATH)
  else
    export SAFERTOS_KERNEL_INSTALL_PATH         = $(SDK_INSTALL_PATH)/safertos_$(SOC)_$(ISA)_$(SAFERTOS_VERSION_$(ISA))
    export SAFERTOS_KERNEL_INSTALL_PATH_$(ISA) ?= $(SDK_INSTALL_PATH)/safertos_$(SOC)_$(ISA)_$(SAFERTOS_VERSION_$(ISA))
  endif
endif