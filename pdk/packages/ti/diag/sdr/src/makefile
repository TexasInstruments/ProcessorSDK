#  ============================================================================
#
# SDR
#
# Software Diagnostics Reference Makefile
#
#  (C) Copyright 2019-2020 Texas Instruments, Inc.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#
#    Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
#    Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the
#    distribution.
#
#    Neither the name of Texas Instruments Incorporated nor the names of
#    its contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
#  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#  ============================================================================

# PDK SDR (Software Diagnostics Reference)
PDK_SDR_DIR = $(PDK_INSTALL_PATH)/ti/diag/sdr

ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

MODULE_NAME = sdr

BUILD_PROFILE ?= release
export BUILD_PROFILE_$(CORE) ?= $(BUILD_PROFILE)

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES =

SRCS_COMMON += sdr_esm.c sdr_ecc.c
SRCS_COMMON += sdr_exception.c sdr_mpuCfg.c
SRCS_COMMON += sdr_ecc_r5.c
SRCS_COMMON += sdr_ccm.c sdr_mpu.c
SRCS_COMMON += sdr_esm_core.c
SRCS_COMMON += sdr_crc.c
SRCS_COMMON += sdr_wdt.c
SRCS_COMMON += sdr_wdt_r5.c
SRCS_ASM_COMMON += sdr_ecc_utils.asm
SRCS_ASM_COMMON += sdr_utils.asm

# Set specific source directories
SRCDIR += src
ifeq ($(SOC),$(filter $(SOC), am65xx j721e))
SRCDIR += src/r5
endif

PACKAGE_SRCS_COMMON = .

# Set specific include directories
INCDIR += $(PDK_SDR_DIR)
INCDIR += $(PDK_SDR_DIR)/src

ifeq ($(SOC),$(filter $(SOC), am65xx j721e))
INCDIR += $(PDK_SDR_DIR)/src/$(SOC)
endif

ifeq ($(CORE),$(filter $(CORE), mcu1_0))
ifeq ($(SOC),$(filter $(SOC), am65xx j721e))
INCDIR += $(PDK_SDR_DIR)/src/$(SOC)/r5
endif
endif

ifdef PDK_CSL2_ROOT_PATH
INCDIR += $(PDK_CSL2_ROOT_PATH)/
else
INCDIR += $(PDK_INSTALL_PATH)/
endif

ifeq ($(CORE),$(filter $(CORE), mcu1_0))
ENDIAN = little
FORMAT = ELF
CGT_PATH = $(TOOLCHAIN_PATH_R5)
CGT_ISA = R5
CGT_EXT = r5f
endif

CFLAGS_LOCAL_COMMON +=

# Core/SoC/platform specific source files and CFLAGS
# Example:
#   SRCS_<core/SoC/platform-name> =
#   CFLAGS_LOCAL_<core/SoC/platform-name> =

# Include common make files
ifeq ($(MAKERULEDIR), )
#Makerule path not defined, define this and assume relative path from ROOTDIR
  MAKERULEDIR := $(ROOTDIR)/ti/build/makerules
  export MAKERULEDIR
endif
include $(MAKERULEDIR)/common.mk

# OBJs and libraries are built by using rule defined in rules_<target>.mk
#     and need not be explicitly specified here

# Nothing beyond this point
