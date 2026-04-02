#
# This file is the makefile for building sbl_lockstep_boot_test to check the lockstep boot functionality
#
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

ifeq ($(CORE), $(filter $(CORE), mcu1_0))
  BUILD_OS_TYPE = freertos
else
  BUILD_OS_TYPE = baremetal
endif
APP_NAME = sbl_lockstep_boot_test
LOCAL_APP_NAME = $(APP_NAME)_$(BOARD)_$(CORE)_testapp

SBL_SRC_DIR =  $(PDK_INSTALL_PATH)/ti/boot/sbl

SRCDIR      += $(PDK_SBL_COMP_PATH)/example/k3MulticoreApp
SRCDIR      += $(PDK_SBL_COMP_PATH)/example/k3MulticoreApp/lockstep_example

INCDIR      += $(PDK_SBL_COMP_PATH)/example/k3MulticoreApp

SOC_DIR=$(SOC)
ifeq ($(SOC), j742s2)
  SOC_DIR=j784s4
endif

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)
PACKAGE_SRCS_COMMON = .

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

# List all the components required by the application
ifeq ($(CORE),$(filter $(CORE), mcu2_0 mcu3_0 mcu4_0))
COMP_LIST_COMMON = $(PDK_COMMON_BAREMETAL_COMP)
endif

SRCS_COMMON += sbl_lockstep_multicore.c sbl_printf.c

# asm files and linker scripts change due to different tool chains for R5 and A53
ifeq ($(CORE),$(filter $(CORE), mcu2_0 mcu3_0 mcu4_0))
  SRCS_ASM_COMMON = sbl_multicore_r5.asm
  EXTERNAL_LNKCMD_FILE_LOCAL =  $(PDK_SBL_COMP_PATH)/example/k3MulticoreApp/$(SOC_DIR)/mcuAmplinker.lds
  APPEND_LNKCMD_FILE = $(PDK_SBL_COMP_PATH)/example/k3MulticoreApp/$(SOC_DIR)/mcuAmplinker_$(CORE).lds
endif

ifeq ($(CORE),$(filter $(CORE), mcu1_0))
  COMP_LIST_COMMON += $(PDK_COMMON_FREERTOS_COMP)
  INCLUDE_EXTERNAL_INTERFACES += freertos
  SRCS_COMMON += main_rtos.c
  COMP_LIST_COMMON += sciserver_tirtos
  EXTERNAL_LNKCMD_FILE_LOCAL =  $(PDK_SBL_COMP_PATH)/example/k3MulticoreApp/linker_mcu1_0.lds
endif

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

