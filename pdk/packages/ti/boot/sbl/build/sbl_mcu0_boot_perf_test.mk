#
# This file is the makefile for building images used for SBL testing.
#
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

EARLY_CAN_TEST_SUFFIX=
# Add a CAN test flag if required
ifeq ($(IS_EARLY_CAN_TEST), yes)
  SBL_CFLAGS += -DEARLY_CAN_TEST
  EARLY_CAN_TEST_SUFFIX=_early_can
endif

HS_TEST_SUFFIX=
# Add a HS Device Only Test Flag if required
ifeq ($(IS_HS_TEST), yes)
  SBL_CFLAGS += -DHS_TEST
  HS_TEST_SUFFIX=_hs
endif

BUILD_OS_TYPE = baremetal
ifeq ($(COMBINED_BOOT_PERF), yes)
  LOCAL_APP_NAME = sbl_$(BUILD_OS_TYPE)_combined_boot_perf$(HS_TEST_SUFFIX)$(EARLY_CAN_TEST_SUFFIX)_$(BOARD)_$(CORE)TestApp
  APP_NAME = sbl_combined_boot_perf$(HS_TEST_SUFFIX)$(EARLY_CAN_TEST_SUFFIX)_test
else
  LOCAL_APP_NAME = sbl_$(BUILD_OS_TYPE)_boot_perf$(HS_TEST_SUFFIX)$(EARLY_CAN_TEST_SUFFIX)_$(BOARD)_$(CORE)TestApp
  APP_NAME = sbl_boot_perf$(HS_TEST_SUFFIX)$(EARLY_CAN_TEST_SUFFIX)_test
endif

SBL_SRC_DIR =  $(PDK_INSTALL_PATH)/ti/boot/sbl

SRCDIR      += $(PDK_SBL_COMP_PATH)/example/k3MulticoreApp

INCDIR      += $(PDK_SBL_COMP_PATH)
INCDIR      += $(PDK_SBL_COMP_PATH)/soc
INCDIR      += $(PDK_SBL_COMP_PATH)/soc/k3
INCDIR      += $(PDK_SBL_COMP_PATH)/example/k3MulticoreApp

# Check for custom flags
ifeq ($(BOOTMODE), cust)
  SBL_CFLAGS = $(CUST_SBL_FLAGS)
endif # ifeq ($(BOOTMODE), cust)

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(SBL_CFLAGS)
PACKAGE_SRCS_COMMON = .

ifeq ($(COMBINED_BOOT_PERF), yes)
  CFLAGS_LOCAL_COMMON += -DCOMBINED_BOOT_PERF
endif

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

# List all the components required by the application
COMP_LIST_COMMON = $(PDK_COMMON_BAREMETAL_COMP) sbl_lib_$(BOOTMODE)

ifeq ($(filter $(SBL_CFLAGS), -DBOOT_MMCSD), -DBOOT_MMCSD)
  COMP_LIST_COMMON += mmcsd fatfs_indp
endif # ifeq ($(filter $(SBL_CFLAGS), -DBOOT_MMCSD), -DBOOT_MMCSD)

ifeq ($(filter $(SBL_CFLAGS), -DBOOT_OSPI), -DBOOT_OSPI)
ifeq ($(filter $(SBL_CFLAGS), -DSBL_USE_DMA=1), -DSBL_USE_DMA=1)
  COMP_LIST_COMMON += spi_dma
else
  COMP_LIST_COMMON += spi
endif # ifeq ($(filter $(SBL_CFLAGS), -DSBL_USE_DMA=1), -DSBL_USE_DMA=1)
endif # ifeq ($(filter $(SBL_CFLAGS), -DBOOT_OSPI), -DBOOT_OSPI)


SRCS_COMMON += sbl_mcu_0_boot_perf_benchmark.c sbl_printf.c
SRCS_ASM_COMMON = sbl_smp_r5.asm

ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
  EXTERNAL_LNKCMD_FILE_LOCAL =  $(PDK_SBL_COMP_PATH)/example/k3MulticoreApp/mcuBootPerfLinker_small.lds
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

