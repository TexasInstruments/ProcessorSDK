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

# File: env.mk. This file contains all the paths and other ENV variables

#
# Module paths
#

# Destination root directory.
#   - specify the directory where you want to place the object, archive/library,
#     binary and other generated files in a different location than source tree
#   - or leave it blank to place then in the same tree as the source
DEST_ROOT ?= $(pdk_PATH)/ti/binary

# Utilities directory. This is required only if the build machine is Windows.
#   - specify the installation directory of utility which supports POSIX commands
#     (eg: Cygwin installation or MSYS installation).
UTILS_INSTALL_DIR ?= $(utils_PATH)

# Set path separator, etc based on the OS
ifeq ($(OS),Windows_NT)
  PATH_SEPARATOR = ;
  UTILSPATH = $(UTILS_INSTALL_DIR)/
  export SHELL := $(UTILSPATH)sh.exe
else
  # else, assume it is linux
  PATH_SEPARATOR = :
  UTILSPATH = /bin/
endif

#Below applicable only for TDA devices
# Radar mmwavelink
radarLink_INCLUDE = $(radarLink_PATH) \
                    $(radarLink_PATH)/ti/control/mmwavelink/include \
                    $(radarLink_PATH)/ti/control/mmwavelink/src     \
                    $(radarLink_PATH)/ti/control/mmwavelink
radarFirmware_INCLUDE = $(radarMssFirmware_PATH) $(radarBssFirmware_PATH)   \
                        $(radarLink_PATH)/firmware
export radarLink_INCLUDE
export radarFirmware_INCLUDE

# PDK
pdk_INCLUDE = $(pdk_PATH)

# Add additional pdk include paths if sub-components are in different directories
ifneq ($(PDK_CSL_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_CSL_ROOT_PATH)
endif

ifneq ($(PDK_CSL2_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_CSL2_ROOT_PATH)
  pdk_INCLUDE += $(PDK_CSL2_ROOT_PATH)/ti/csl/arch/r5
  pdk_INCLUDE += $(PDK_CSL2_ROOT_PATH)/ti/csl
endif

ifneq ($(PDK_SA_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_SA_ROOT_PATH)
endif
ifneq ($(PDK_PM_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_PM_ROOT_PATH)
endif
ifneq ($(PDK_PMIC_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_PMIC_ROOT_PATH)
endif
ifneq ($(PDK_OSAL_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_OSAL_ROOT_PATH)
endif
ifneq ($(PDK_I2C_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_I2C_ROOT_PATH)
endif
ifneq ($(PDK_GPIO_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_GPIO_ROOT_PATH)
endif
ifneq ($(PDK_FATFS_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_FATFS_ROOT_PATH)
endif
ifneq ($(PDK_MMCSD_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_MMCSD_ROOT_PATH)
endif
ifneq ($(PDK_MCASP_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_MCASP_ROOT_PATH)
endif
ifneq ($(PDK_PROFILING_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_PROFILING_ROOT_PATH)
endif
ifneq ($(PDK_PRUSS_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_PRUSS_ROOT_PATH)
endif
ifneq ($(PDK_ICSS_EMAC_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_ICSS_EMAC_ROOT_PATH)
endif
ifneq ($(PDK_EMAC_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_EMAC_ROOT_PATH)
endif
ifneq ($(PDK_UART_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_UART_ROOT_PATH)
endif
ifneq ($(PDK_SPI_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_SPI_ROOT_PATH)
endif
ifneq ($(PDK_BOARD_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_BOARD_ROOT_PATH)
endif
ifneq ($(PDK_BOARD_DIAG_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_BOARD_DIAG_ROOT_PATH)
endif
ifneq ($(PDK_BOARD_UTILS_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_BOARD_UTILS_ROOT_PATH)
endif

#Below applicable only for K3 devices
ifneq ($(PDK_LPM_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_LPM_ROOT_PATH)
endif
ifneq ($(PDK_UDMA_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_UDMA_ROOT_PATH)
endif
ifneq ($(PDK_ENET_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_ENET_ROOT_PATH)
endif
ifneq ($(PDK_LWIP_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_LWIP_ROOT_PATH)
endif
ifneq ($(PDK_CSIRX_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_CSIRX_ROOT_PATH)
endif
ifneq ($(PDK_CSITX_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_CSITX_ROOT_PATH)
endif
ifneq ($(PDK_DSS_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_DSS_ROOT_PATH)
endif
ifneq ($(PDK_SCICLIENT_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_SCICLIENT_ROOT_PATH)
endif
ifneq ($(PDK_IPC_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_IPC_ROOT_PATH)
endif
ifneq ($(PDK_FVID2_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_FVID2_ROOT_PATH)
endif
ifneq ($(SAFETY_CHECKERS_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(SAFETY_CHECKERS_ROOT_PATH)
endif
ifneq ($(IP_FMA_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(IP_FMA_ROOT_PATH)
endif

#Below are used for TDA/DRA SBL
ifneq ($(PDK_STW_LLD_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_STW_LLD_ROOT_PATH)
endif
ifneq ($(PDK_NORFLASH_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_NORFLASH_ROOT_PATH)
endif
ifneq ($(PDK_QSPIFLASH_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_QSPIFLASH_ROOT_PATH)
endif

#Below applicable only for TPR12 devices
ifneq ($(PDK_MAILBOX_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_MAILBOX_ROOT_PATH)
endif
ifneq ($(PDK_CAN_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_CAN_ROOT_PATH)
endif
ifneq ($(PDK_GPADC_ROOT_PATH), $(pdk_PATH))
  pdk_INCLUDE += $(PDK_GPADC_ROOT_PATH)
endif

export pdk_INCLUDE

include $(pdk_PATH)/ti/build/makerules/component.mk

# Package Relative path and destination directory
PACKAGE_RELPATH = pdk_/packages
PACKAGE_ROOT = $(DEST_ROOT)/package/$(PACKAGE_SELECT)/$(PACKAGE_RELPATH)
export PACKAGE_RELPATH
export PACKAGE_ROOT

#
# Tools paths
#

# Commands commonly used within the make files
RM = $(UTILSPATH)rm -f
MV = $(UTILSPATH)mv
RMDIR = $(UTILSPATH)rm -rf
MKDIR = $(UTILSPATH)mkdir
ECHO = @$(UTILSPATH)echo
CAT	= $(UTILSPATH)cat

#emake provides detailed info for build analysis.
EMAKE = emake --emake-emulation=gmake --emake-gen-subbuild-db=1 --emake-annodetail=waiting

ifeq ($(OS),Windows_NT)
  MAKE = gmake
else
  MAKE = make
endif

EGREP = $(UTILSPATH)egrep
CP = $(UTILSPATH)cp
ifeq ($(OS),Windows_NT)
  CHMOD = $(UTILSPATH)echo
else
  CHMOD = $(UTILSPATH)chmod
endif

ifeq ($(BUILD_OS_TYPE),freertos)
  ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
      CONFIG_BLD_LNK_r5f   = $(pdk_PATH)/ti/build/$(SOC)/linker_r5_freertos.lds
  endif
  ifeq ($(SOC),$(filter $(SOC), j721e))
      CONFIG_BLD_LNK_c66   = $(pdk_PATH)/ti/build/$(SOC)/linker_c66_freertos.cmd
  endif
  ifeq ($(SOC),$(filter $(SOC), j721e j721s2 j784s4 j742s2))
      CONFIG_BLD_LNK_c7x   = $(pdk_PATH)/ti/build/$(SOC)/linker_c7x_freertos.cmd
  endif
endif

ifeq ($(BUILD_OS_TYPE),safertos)
  ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
      CONFIG_BLD_LNK_r5f   = $(pdk_PATH)/ti/build/$(SOC)/linker_r5_safertos.lds
  endif
  ifeq ($(SOC),$(filter $(SOC), j721e j721s2 j784s4 j742s2))
      CONFIG_BLD_LNK_c7x   = $(pdk_PATH)/ti/build/$(SOC)/linker_c7x_safertos.cmd
  endif
  ifeq ($(SOC),$(filter $(SOC), j721e))
      CONFIG_BLD_LNK_c66   = $(pdk_PATH)/ti/build/$(SOC)/linker_c66_safertos.cmd
  endif
endif

ifeq ($(BUILD_OS_TYPE),baremetal)
  ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
      CONFIG_BLD_LNK_r5f   = $(pdk_PATH)/ti/build/$(SOC)/linker_r5.lds
      CONFIG_BLD_LNK_a72   = $(pdk_PATH)/ti/build/$(SOC)/linker_a72_mpu1_0.lds
      CONFIG_BLD_LNK_c66   = $(pdk_PATH)/ti/build/$(SOC)/linker_c66.cmd
  endif
endif

export CGTOOLS_A72 = $(TOOLCHAIN_PATH_A72)

STRIP_ALL_ARM = $(CODEGEN_PREFIX)strip -s
STRIP_DEBUG_ARM = $(CODEGEN_PREFIX)strip --strip-debug

# Nothing beyond this point
