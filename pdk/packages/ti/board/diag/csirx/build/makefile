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
ifeq ($(IS_BAREMETAL),yes)
DIAGNAME ?= csirx
else
DIAGNAME ?= csirx_tirtos
endif
PROFILE ?= release
TESTMODE ?= FUNCTIONAL_TEST
MODENAME ?=
FE_DEVICE ?= AWR2243
BOARD_DIAG_CFLAGS ?=
APP_NAME ?= board_diag_$(DIAGNAME)

BUILD_PROFILE_$(CORE) = $(PROFILE)

ifeq ($(IS_BAREMETAL),yes)
BUILD_OS_TYPE = baremetal
CFLAGS_OS_DEFINES =
LNKFLAGS_LOCAL_mpu1_0  += --entry Entry
COMP_LIST_COMMON = $(PDK_COMMON_BAREMETAL_COMP)
EXTERNAL_INTERFACES =
XDC_CFG_FILE_mpu1_0 =
XDC_CFG_FILE_mcu1_0 =
else
BUILD_OS_TYPE = tirtos
CFLAGS_OS_DEFINES = -DUSE_BIOS
EXTERNAL_INTERFACES = bios xdc
COMP_LIST_COMMON    = $(PDK_COMMON_TIRTOS_COMP) 


ifeq ($(SOC),$(filter $(SOC), tpr12))
# Enable XDC build for application by providing XDC CFG File per core
XDC_CFG_FILE_mcu1_0 = $(PDK_INSTALL_PATH)/ti/build/$(SOC)/sysbios_r5f.cfg
XDC_CFG_FILE_c66xdsp_1 = $(PDK_INSTALL_PATH)/ti/build/$(SOC)/sysbios_c66.cfg
XDC_CFG_UPDATE_$(CORE) = ../../common/$(SOC)/osal_log_config.xs
endif

endif

ifeq ($(MODENAME), )
ifeq ($(TESTMODE), STRESS_TEST)
MODENAME = Stress
endif
endif

SRCDIR = ../src  ../../common/$(SOC) ./$(BOARD)
INCDIR = ../../../../board ../src ../../../src/$(BOARD)/include ../../../src/devices ../../common/$(SOC) ../../../../csl

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk $(EXTERNAL_INTERFACES)


ifeq ($(SOC), $(filter $(SOC), tpr12))
COMP_LIST_COMMON += csirx
else
COMP_LIST_COMMON += fvid2 csirx dss
endif

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON = makefile
PACKAGE_SRCS_COMMON += ../../common/$(SOC)
PACKAGE_SRCS_COMMON += ../../board_diag_component.mk
PACKAGE_SRCS_COMMON += ../../create_sd.bat ../../create_sd.sh

ifeq ($(SOC), $(filter $(SOC), tpr12))
PACKAGE_SRCS_COMMON += ../src/csirx_test_tpr12.c ../src/csirx_test_tpr12.h
#AWR2243/IWR1443 configuration binaries
PACKAGE_SRCS_COMMON += ../frontendcfg
SRCS_COMMON += csirx_test_tpr12.c
else
PACKAGE_SRCS_COMMON += ../src/csirx_display.c ../src/csirx_display.h ../src/csirx_test.c ../src/csirx_test.h
SRCS_COMMON += csirx_test.c csirx_display.c
endif

SRCS_COMMON += diag_common_cfg.c

ifeq ($(IS_BAREMETAL),yes)
    ifeq ($(SOC), $(filter $(SOC), tpr12))
        ifeq ($(CORE),$(filter $(CORE), mcu1_0))
            SRCS_ASM_COMMON += diag_entry_r5.asm
            EXTERNAL_LNKCMD_FILE_LOCAL = ../../common/$(SOC)/linker_$(CORE).lds
        else
            EXTERNAL_LNKCMD_FILE_LOCAL = ../../common/$(SOC)/linker_$(CORE).cmd
        endif
    endif
else
#sysbios case
    ifeq ($(SOC), $(filter $(SOC), tpr12))
        ifeq ($(CORE),$(filter $(CORE), c66xdsp_1))
            EXTERNAL_LNKCMD_FILE_LOCAL = ../../common/$(SOC)/linker_c66xdsp_1_sysbios.cmd
        endif
        ifeq ($(CORE),$(filter $(CORE), mcu1_0))
            EXTERNAL_LNKCMD_FILE_LOCAL = ../../common/$(SOC)/linker_mcu1_0_sysbios.lds
        endif
    endif
endif

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) -DDIAG_$(TESTMODE) $(BOARD_DIAG_CFLAGS) $(CFLAGS_OS_DEFINES) -DFE_$(FE_DEVICE)

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
