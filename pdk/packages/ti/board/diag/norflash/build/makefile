# Copyright (C) 2019-2020 Texas Instruments Incorporated - http://www.ti.com/
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

# Board diag parameters which can be controlled outside this makefile
DIAGNAME ?= norflash
PROFILE ?= release
TESTMODE ?= FUNCTIONAL_TEST
MODENAME ?=
BOARD_DIAG_CFLAGS ?=
APP_NAME ?= board_diag_$(DIAGNAME)

BUILD_PROFILE_$(CORE) = $(PROFILE)

BUILD_OS_TYPE = baremetal
LNKFLAGS_LOCAL_mpu1_0  += --entry Entry
EXTERNAL_INTERFACES =
XDC_CFG_FILE_mpu1_0 =
XDC_CFG_FILE_mcu1_0 =

ifeq ($(MODENAME), )
ifeq ($(TESTMODE), STRESS_TEST)
MODENAME = Stress
endif
endif

SRCDIR = $(PDK_INSTALL_PATH)/ti/board/diag/$(DIAGNAME)/src $(PDK_INSTALL_PATH)/ti/board/diag/$(DIAGNAME)/build/$(BOARD) $(PDK_INSTALL_PATH)/ti/board/diag/common/$(SOC) $(PDK_INSTALL_PATH)/ti/csl
INCDIR = $(PDK_INSTALL_PATH)/ti/board/ $(PDK_INSTALL_PATH)/ti/csl $(PDK_INSTALL_PATH)/ti/board/diag/$(DIAGNAME)/src $(PDK_INSTALL_PATH)/ti/board/src/$(BOARD)/include $(PDK_INSTALL_PATH)/ti/board/diag/common/$(SOC)
SRCDIR += ./$(BOARD)
INCDIR += ./$(BOARD)

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

# List all the components required by the application
COMP_LIST_COMMON = $(PDK_COMMON_BAREMETAL_COMP)
COMP_LIST_COMMON += spi
ifeq ($(BOARD), $(filter $(BOARD), am64x_svb))
COMP_LIST_COMMON += gpmc
endif

# Common source files and CFLAGS across all platforms and cores
ifeq ($(BOARD), $(filter $(BOARD), am65xx_evm am65xx_idk))
PACKAGE_SRCS_COMMON = ../src/am65xx_spi_test.c ../src/am65xx_spi_test.h
else ifeq ($(BOARD), $(filter $(BOARD), j7200_evm))
PACKAGE_SRCS_COMMON = ../src/spi_norflash_test.c ../src/spi_norflash_test.h
else ifeq ($(BOARD), $(filter $(BOARD), tpr12_evm awr294x_evm))
PACKAGE_SRCS_COMMON = ../src/spi_test.c ../src/spi_test.h
else
PACKAGE_SRCS_COMMON = ../src/ospi_quad_test.c ../src/ospi_quad_test.h
endif
PACKAGE_SRCS_COMMON += ../../common/$(SOC) makefile
PACKAGE_SRCS_COMMON += ../../board_diag_component.mk
PACKAGE_SRCS_COMMON += ../../create_sd.bat ../../create_sd.sh

ifeq ($(BOARD), $(filter $(BOARD), am65xx_evm am65xx_idk))
SRCS_COMMON += am65xx_spi_test.c
else ifeq ($(BOARD), $(filter $(BOARD), j7200_evm))
SRCS_COMMON += spi_norflash_test.c
else ifeq ($(BOARD), $(filter $(BOARD), tpr12_evm awr294x_evm))
SRCS_COMMON += spi_test.c
else
SRCS_COMMON += ospi_quad_test.c
endif

SRCS_COMMON += diag_common_cfg.c

ifeq ($(CORE),$(filter $(CORE), mcu1_0))
SRCS_ASM_COMMON += diag_entry_r5.asm
EXTERNAL_LNKCMD_FILE_LOCAL = ../../common/$(SOC)/linker_$(CORE).lds
endif

ifeq ($(CORE),$(filter $(CORE), mpu1_0))
SRCS_ASM_COMMON += diag_entry.asm
EXTERNAL_LNKCMD_FILE_LOCAL = ../../common/$(SOC)/linker_$(CORE).lds
endif

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) -DDIAG_$(TESTMODE) $(BOARD_DIAG_CFLAGS)

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
