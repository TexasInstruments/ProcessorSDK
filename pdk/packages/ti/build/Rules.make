#
# Copyright (c) 2013-2024, Texas Instruments Incorporated
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# *  Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
# *  Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# *  Neither the name of Texas Instruments Incorporated nor the names of
#    its contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

#Default build environment, (Windows_NT / linux)
#if nothing is defined, default to linux as in windows this variable is defined
export OS ?= linux

SDK_INSTALL_PATH ?= $(abspath ../../../../)
TOOLS_INSTALL_PATH ?= $(SDK_INSTALL_PATH)

#Default BUILD_OS_TYPE (tirtos/baremetal/freertos)
export BUILD_OS_TYPE ?= freertos

include $(PDK_INSTALL_PATH)/ti/build/procsdk_defs.mk

################################################################################
# Other user configurable variables
################################################################################

#if LIMIT_BOARDS is defined, BOARD from the LIMIT_BOARDS
ifneq ($(LIMIT_BOARDS),)
BOARD ?= $(firstword $(LIMIT_BOARDS))
SOC ?= $(firstword $(LIMIT_SOCS))
CORE ?= $(firstword $(LIMIT_CORES))
else
#if LIMIT_BOARDS is not defined, default BOARD and SOC to the below
export BOARD ?= j721e_evm
export SOC ?= j721e
endif
# Default to m4 build depending on BOARD selected!!
ifeq ($(BOARD),$(filter $(BOARD),j721e_evm j7200_evm j721s2_evm j784s4_evm j742s2_evm))
  CORE ?= mcu1_0
endif
ifeq ($(BOARD),$(filter $(BOARD), j721e_hostemu j7200_hostemu j721s2_hostemu j784s4_hostemu j742s2_hostemu))
  CORE = c7x-hostemu
endif


# Default Build Profile
# Supported Values: debug | release
export BUILD_PROFILE ?= release

# Default compile mode for R5 is thumb
# Supported Values: thumb
export COMPILE_MODE ?= thumb

# Default Instrumentation flag is false
# Supported Values: false true
export INSTRUMENTATION_FLAG ?= false

# Treat compiler warning as error
# Supported Values: yes | no
ifeq ($(INSTRUMENTATION_FLAG),true)
export TREAT_WARNINGS_AS_ERROR ?= no
else
export TREAT_WARNINGS_AS_ERROR ?= yes
endif

#Various boards support for J7 TDA family of devices
BOARD_LIST_J7_TDA = j721e_hostemu j721e_evm
BOARD_LIST_J7_TDA += j7200_hostemu j7200_evm
BOARD_LIST_J7_TDA += j721s2_evm j721s2_hostemu
BOARD_LIST_J7_TDA += j784s4_evm j784s4_hostemu
BOARD_LIST_J7_TDA += j742s2_evm j742s2_hostemu
export BOARD_LIST_J7_TDA

################################################################################
# Other advanced configurable variables
################################################################################

# Set Core Build Profile depending on BUILD_PROFILE value
export BUILD_PROFILE_$(CORE) ?= $(BUILD_PROFILE)

# Default PACKAGE_SELECT build flag
# Supported values: all, vps-hal-only, vps-vip-only, vps-vpe-only, vps-dss-only, vps-vip-dss, vps-vip-vpe
export PACKAGE_SELECT ?= all

# Disable recursive building of example dependencies
ifeq ($(BUILD_PROFILE),release)
  #Below is converted to yes in release script
  DISABLE_RECURSE_DEPS ?= no
else
  #We don't generate debug lib. So force recursive build for debug build.
  DISABLE_RECURSE_DEPS ?= no
endif
export DISABLE_RECURSE_DEPS

# Default C++ build flag, yes or no
export CPLUSPLUS_BUILD ?= no

include $(PDK_INSTALL_PATH)/ti/build/pdk_tools_path.mk

#safertos_package_path.mk will be packaged only for SOCs with SafeRTOS Build is supported.
#Hence, include only if available
ifneq ($(wildcard $(PDK_INSTALL_PATH)/ti/build/safertos_package_path.mk),)
  include $(PDK_INSTALL_PATH)/ti/build/safertos_package_path.mk
endif

#use <module>_PATH variable as makefile internally expects PATH variable this way for external component path
export pdk_PATH := $(PDK_INSTALL_PATH)
export radarLink_PATH := $(RADARLINK_INSTALL_PATH)
export ipc_PATH := $(IPC_INSTALL_PATH)
export uia_PATH := $(UIA_INSTALL_PATH)

export ROOTDIR := $(pdk_PATH)

#Default SECTTI SIZE INFORMATION
export SECTTI_SIZE_INFO ?= no

#Default SECTTI tool
export SECTTI ?= $(CG_XML_BIN_INSTALL_PATH)/sectti

# Build for HS devices if secdev package is found
ifneq ("$(wildcard $(TI_SECURE_DEV_PKG)/scripts/secure-binary-image.sh)","")
  export SECUREMODE ?= yes
endif

# include other dependent files
include $(PDK_INSTALL_PATH)/ti/build/comp_paths.mk
ifeq ($(MAKERULEDIR), )
  #Makerule path not defined, define this and assume relative path from ROOTDIR
  export MAKERULEDIR := $(ROOTDIR)/ti/build/makerules
endif
include $(MAKERULEDIR)/build_config.mk
include $(MAKERULEDIR)/platform.mk
include $(MAKERULEDIR)/env.mk

################################################################################
# Build Tools Configuration
################################################################################

ifeq ($(OS),Windows_NT)
  PATH := $(PATH)
endif

# Compiler Tools:
# PATH := $(C6X_GEN_INSTALL_PATH)/bin;$(PATH)

ifeq ($(OS),Windows_NT)
  PATH := $(subst /,\,$(PATH))
else
  PATH := $(subst ;,:,$(PATH))
endif

export PATH

LIBDIR ?= ./lib
export LIBDIR