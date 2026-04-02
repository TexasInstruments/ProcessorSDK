#
# This file is the makefile for building images used for SBL testing.
#
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

ifeq ($(IS_BOOTAPP_TEST), yes)
  APP_NAME = bootapp_boot_test
else
  APP_NAME = sbl_boot_test
endif

ifneq ($(CORE), $(filter $(CORE), mcu1_0 c7x_1 c7x_2 c7x_3 c7x_4 c66xdsp_1 c66xdsp_2))
  BUILD_OS_TYPE = baremetal
else
  BUILD_OS_TYPE = freertos
endif
LOCAL_APP_NAME = $(APP_NAME)_$(BOARD)_$(CORE)TestApp

SBL_SRC_DIR =  $(PDK_INSTALL_PATH)/ti/boot/sbl

SRCDIR      += $(PDK_SBL_COMP_PATH)/example/k3MulticoreApp

INCDIR      += $(PDK_SBL_COMP_PATH)/example/k3MulticoreApp
INCDIR      += $(PDK_INSTALL_PATH)

SOC_DIR=$(SOC)
ifeq ($(SOC), j742s2)
  SOC_DIR=j784s4
endif

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)
PACKAGE_SRCS_COMMON = .

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES =

# List all the components required by the application
COMP_LIST_COMMON =

ifeq ($(IS_BOOTAPP_TEST), yes)
  CFLAGS_LOCAL_COMMON += -DBOOTAPP_TEST
endif

SRCS_COMMON += sbl_amp_multicore.c sbl_printf.c

# asm files and linker scripts change due to different tool chains for R5 and A53
ifeq ($(CORE),$(filter $(CORE), mcu1_1 mcu2_0 mcu2_1 mcu3_0 mcu3_1 mcu4_0 mcu4_1))
  SRCS_ASM_COMMON = sbl_multicore_r5.asm
  EXTERNAL_LNKCMD_FILE_LOCAL =  $(PDK_SBL_COMP_PATH)/example/k3MulticoreApp/$(SOC_DIR)/mcuAmplinker.lds
  APPEND_LNKCMD_FILE = $(PDK_SBL_COMP_PATH)/example/k3MulticoreApp/$(SOC_DIR)/mcuAmplinker_$(CORE).lds
endif

ifeq ($(CORE),$(filter $(CORE), mpu1_0 mpu1_1 mpu1_2 mpu1_3 mpu2_0 mpu2_1 mpu2_2 mpu2_3))
  SRCS_ASM_COMMON = sbl_multicore_a53.asm
  LNKCMD_FILE = $(PDK_SBL_COMP_PATH)/example/k3MulticoreApp/$(SOC_DIR)/mpuAmplinker.lds
endif

ifeq ($(CORE),$(filter $(CORE), mcu1_0 c7x_1 c7x_2 c7x_3 c7x_4 c66xdsp_1 c66xdsp_2))
  COMP_LIST_COMMON += $(PDK_COMMON_FREERTOS_COMP)
  INCLUDE_EXTERNAL_INTERFACES += freertos
  SRCS_COMMON += main_rtos.c
  ifeq ($(CORE), mcu1_0)
    COMP_LIST_COMMON += sciserver_tirtos
    EXTERNAL_LNKCMD_FILE_LOCAL =  $(PDK_SBL_COMP_PATH)/example/k3MulticoreApp/linker_mcu1_0.lds
  else
    EXTERNAL_LNKCMD_FILE_LOCAL =  $(PDK_SBL_COMP_PATH)/example/k3MulticoreApp/$(SOC_DIR)/dspAMPlinker_$(CORE).lds
  endif
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

