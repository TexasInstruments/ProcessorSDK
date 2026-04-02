################################################################################
# Configure toolchain paths
################################################################################
  # This section applies to Processor SDK RTOS release
  PDK_VERSION_STR=_$(PDK_SOC)_$(PDK_VERSION)
ifeq ($(PDK_VERSION),)
  PDK_VERSION_STR=
endif

  #Tool versions
  GCC_ARCH64_VERSION=9.2-2019.12
  GCC_ARCH64_BIN_PREFIX_STR=aarch64-none-elf
  CGT_VERSION=8.3.7
  CGT_C7X_VERSION=5.0.0.LTS
  CGT_ARMLLVM_VERSION=4.0.4.LTS
  CG_XML_VERSION=2.61.00

################################################################################
# Dependent toolchain paths variables
################################################################################
  export C6X_GEN_INSTALL_PATH      ?= $(TOOLS_INSTALL_PATH)/ti-cgt-c6000_$(CGT_VERSION)
  export C7X_GEN_INSTALL_PATH      ?= $(TOOLS_INSTALL_PATH)/ti-cgt-c7000_$(CGT_C7X_VERSION)

ifeq ($(OS),Windows_NT)
  #Paths for windows machine
  export TOOLCHAIN_PATH_GCC_ARCH64 ?= $(TOOLS_INSTALL_PATH)/gcc-arm-$(GCC_ARCH64_VERSION)-mingw-w64-i686-aarch64-none-elf
else
  #Paths for linux machine
  export TOOLCHAIN_PATH_GCC_ARCH64 ?= $(TOOLS_INSTALL_PATH)/gcc-arm-$(GCC_ARCH64_VERSION)-x86_64-aarch64-none-elf
endif
  export GCC_ARCH64_BIN_PREFIX        ?= $(GCC_ARCH64_BIN_PREFIX_STR)
  export TOOLCHAIN_PATH_A72           ?= $(TOOLCHAIN_PATH_GCC_ARCH64)
  export TOOLCHAIN_PATH_R5            ?= $(TOOLS_INSTALL_PATH)/ti-cgt-armllvm_$(CGT_ARMLLVM_VERSION)
  export PDK_INSTALL_PATH             ?= $(SDK_INSTALL_PATH)/pdk$(PDK_VERSION_STR)/packages
  export CG_XML_BIN_INSTALL_PATH      ?= $(SDK_INSTALL_PATH)/cg_xml_$(CG_XML_VERSION)/bin
  export FREERTOS_KERNEL_INSTALL_PATH ?= $(PDK_INSTALL_PATH)/ti/kernel/freertos/FreeRTOS-LTS
  export FREERTOS_LABS_INSTALL_PATH   ?= $(PDK_INSTALL_PATH)/ti/kernel/freertos/FreeRTOS-Labs

# Utilities directory. This is required only if the build machine is Windows.
# This should be CCS install directory as in c:/ti/ccsv<ver>/ccs/utils/cygwin
ifeq ($(OS),Windows_NT)
  export utils_PATH ?= c:/ti/ccs1120/ccs/utils/cygwin
endif
