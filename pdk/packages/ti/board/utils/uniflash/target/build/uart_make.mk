# Copyright (C) 2019-2022 Texas Instruments Incorporated - http://www.ti.com/
#
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
# Macro definitions referenced below
#

#
# This file is the makefile for building CSL uart app.
#
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

# Uniflash parameters which can be controlled outside this makefile
UNIFLASH ?= uart
PROFILE ?= release
BOARD_UTILS_CFLAGS ?=

HS_SUFFIX=
ifeq ($(BUILD_HS),yes)
HS_SUFFIX=_hs
endif

APP_NAME ?= board_utils_$(UNIFLASH)_flash_programmer$(HS_SUFFIX)

BUILD_PROFILE_$(CORE) = $(PROFILE)

BUILD_OS_TYPE = baremetal
LNKFLAGS_LOCAL_mpu1_0  += --entry Entry
EXTERNAL_INTERFACES =
XDC_CFG_FILE_mpu1_0 =
XDC_CFG_FILE_mcu1_0 =

UNIFLASH_BASE_DIR=$(PDK_BOARD_UTILS_COMP_PATH)/uniflash/target

# Name of the binary if different from the default (APP_NAME)_$(BOARD_$(CORE)_<build_profile>
LOCAL_APP_NAME = $(UNIFLASH)_$(BOARD)_flash_programmer

SRCDIR = $(UNIFLASH_BASE_DIR)/src
INCDIR = $(PDK_BOARD_COMP_PATH) $(UNIFLASH_BASE_DIR)/src $(UNIFLASH_BASE_DIR)/include $(PDK_BOARD_COMP_PATH)/src/$(BOARD)/include $(PDK_BOARD_COMP_PATH)/src/$(BOARD) $(PDK_BOARD_COMP_PATH)/src/flash $(PDK_CSL_COMP_PATH)

ifeq ($(BOARD), $(filter $(BOARD), j721e_evm am65xx_evm am65xx_idk j7200_evm am64x_evm j721s2_evm j784s4_evm j742s2_evm))
  SRCDIR += $(UNIFLASH_BASE_DIR)/soc/k3 $(UNIFLASH_BASE_DIR)/src/ospi $(UNIFLASH_BASE_DIR)/src/qspi $(UNIFLASH_BASE_DIR)/src/hyperflash $(UNIFLASH_BASE_DIR)/src/emmc
  INCDIR += $(UNIFLASH_BASE_DIR)/soc/k3 $(UNIFLASH_BASE_DIR)/src/ospi $(UNIFLASH_BASE_DIR)/src/qspi $(UNIFLASH_BASE_DIR)/src/hyperflash $(UNIFLASH_BASE_DIR)/src/emmc
endif

ifeq ($(BOARD), $(filter $(BOARD), tpr12_evm awr294x_evm))
  SRCDIR += $(UNIFLASH_BASE_DIR)/soc/$(SOC) $(UNIFLASH_BASE_DIR)/src/qspi
  INCDIR += $(UNIFLASH_BASE_DIR)/src/qspi
endif

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

# List all the components required by the application
COMP_LIST_COMMON = $(PDK_COMMON_BAREMETAL_COMP)
ifeq ($(BOARD), $(filter $(BOARD), j721e_evm am65xx_evm am65xx_idk j7200_evm am64x_evm j721s2_evm j784s4_evm j742s2_evm))
COMP_LIST_COMMON += spi_dma mmcsd
endif

ifeq ($(BOARD), $(filter $(BOARD), tpr12_evm awr294x_evm))
COMP_LIST_COMMON += spi_dma
endif

ifneq ($(strip $(HS_SUFFIX)),) #if $(HS_SUFFIX) is non-empty
  SCICLIENT := $(filter sciclient%,$(COMP_LIST_COMMON))
  COMP_LIST_COMMON := $(subst $(SCICLIENT),$(SCICLIENT)_hs,$(COMP_LIST_COMMON))
endif

# Common source files and CFLAGS across all platforms and cores
ifeq ($(BUILD_HS),yes)
  # Already packaged in non HS build. Relative paths are different. So better to skip
  PACKAGE_SRCS_COMMON =
else
  PACKAGE_SRCS_COMMON = ../../board ../../build ../../include ../../src ../../soc/soc.h
  PACKAGE_SRCS_COMMON += ../../../host
  PACKAGE_SRCS_COMMON += ../../../../board_utils_component.mk
  ifeq ($(BOARD), $(filter $(BOARD), j721e_evm am65xx_evm am65xx_idk j7200_evm am64x_evm j721s2_evm j784s4_evm j742s2_evm))
  PACKAGE_SRCS_COMMON += ../../soc/k3
  endif
  ifeq ($(BOARD), $(filter $(BOARD), tpr12_evm awr294x_evm))
  PACKAGE_SRCS_COMMON += ../../soc/$(SOC)
  endif
  PACKAGE_SRCS_COMMON = ../../target ../../host ../../../board_utils_component.mk
endif

SRCS_COMMON = uart_main.c xmodem.c soc.c

ifeq ($(BOARD), $(filter $(BOARD), j721e_evm j7200_evm))
SRCS_COMMON += ospi.c emmc.c hyperflash.c
EXTERNAL_LNKCMD_FILE_LOCAL = $(UNIFLASH_BASE_DIR)/soc/k3/linker_j7.cmd
endif

ifeq ($(BOARD), $(filter $(BOARD), j721s2_evm j784s4_evm j742s2_evm))
SRCS_COMMON += ospi.c ospi_nand.c emmc.c
SRCS_ASM_COMMON += ufp_misc.asm
EXTERNAL_LNKCMD_FILE_LOCAL = $(UNIFLASH_BASE_DIR)/soc/k3/linker_j7.cmd
INCDIR += $(UNIFLASH_BASE_DIR)/src/ospi_nand
SRCDIR += $(UNIFLASH_BASE_DIR)/src/ospi_nand
endif

ifeq ($(BOARD), $(filter $(BOARD), j721e_evm j721s2_evm j784s4_evm j742s2_evm))
SRCS_COMMON += qspi.c
endif
ifeq ($(BOARD), $(filter $(BOARD), am65xx_evm am65xx_idk am64x_evm))
SRCS_COMMON += ospi.c emmc.c
EXTERNAL_LNKCMD_FILE_LOCAL = $(UNIFLASH_BASE_DIR)/soc/k3/linker_$(SOC).cmd
endif

ifeq ($(BOARD), $(filter $(BOARD), am64x_evm))
SRCS_ASM_COMMON += ufp_init.asm ufp_misc.asm
endif

ifeq ($(BOARD), $(filter $(BOARD), am65xx_evm am65xx_idk j721e_evm j7200_evm))
SRCS_ASM_COMMON += ufp_misc.asm
endif

ifeq ($(BOARD), $(filter $(BOARD), tpr12_evm awr294x_evm))
SRCS_COMMON += qspi.c
EXTERNAL_LNKCMD_FILE_LOCAL = $(UNIFLASH_BASE_DIR)/soc/$(SOC)/linker.cmd
BOARD_UTILS_CFLAGS += -DSPI_DMA_ENABLE
endif

ifeq ($(NOJTAG), yes)
BOARD_UTILS_CFLAGS += UFP_DISABLE_JTAG_LOAD
endif

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(BOARD_UTILS_CFLAGS) -DSOC_$(SOC)
ifeq ($(BOARD), $(filter $(BOARD), am65xx_evm am65xx_idk))
CFLAGS_LOCAL_COMMON += -DSOC_AM65XX
endif

ifeq ($(BUILD_HS),yes)
CFLAGS_LOCAL_COMMON += -DBUILD_HS
endif

ifeq ($(BOARD), $(filter $(BOARD), j721e_evm am65xx_evm am65xx_idk am64x_evm j721s2_evm j784s4_evm j742s2_evm))
ifeq ($(BUILD_HS),no)
#CFLAGS_LOCAL_COMMON += -DSPI_DMA_ENABLE
endif
endif

SBL_OBJ_COPY := $(TOOLCHAIN_PATH_GCC_ARCH64)/bin/$(GCC_ARCH64_BIN_PREFIX)-objcopy
#GCC toolchain is not part of SDK release and need to be downloaded seperately.
#Check for gcc toolchain in the SDK path and throw error if it is not present
ifeq ("$(wildcard ${TOOLCHAIN_PATH_GCC_ARCH64})","")
$(error gcc-arm compiler not found. please download it by running setup_psdk_rtos.sh)
endif
export SBL_OBJ_COPY

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
