#
# This file is the makefile for building CSL crc app.
#
#   Copyright (c) Texas Instruments Incorporated 2017 - 2022
#
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

BUILD_OS_TYPE=baremetal

APP_NAME = csl_ecc_test_app

SRCDIR = .
INCDIR =

ifeq ($(SOC), $(filter $(SOC), k2g k2e k2k k2h k2l c6657 c6678))
SRCDIR += $(PDK_INSTALL_PATH)/ti/csl/arch/$(CORE)/src
INCDIR += $(PDK_INSTALL_PATH)/ti/csl/arch/$(CORE)
INCDIR += $(PDK_INSTALL_PATH)
endif
# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk csl_arch

# List all the components required by the application
ifeq ($(BOARD),$(filter $(BOARD), evmK2G evmK2K evmK2H evmK2L evmK2E evmC6657 evmC6678 evmAM572x idkAM571x idkAM572x idkAM574x am65xx_evm j721e_evm j7200_evm am64x_evm j721s2_evm j784s4_evm j742s2_evm))
 COMP_LIST_COMMON += $(PDK_COMMON_BAREMETAL_COMP)
else
 COMP_LIST_COMMON += csl
 ifeq ($(CORE),$(filter $(CORE), a15_0))
  COMP_LIST_a15_0 = csl_init
 endif
endif

ifeq ($(SOC), $(filter $(SOC), k2g k2e k2k k2h k2l c6657 c6678))
COMP_LIST_COMMON += csl_uart_console
else
COMP_LIST_COMMON += csl_utils_common csl_uart_console
endif
ifeq ($(CORE),$(filter $(CORE), a15_0))
COMP_LIST_COMMON += csl_arch
endif


# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON = .

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)

ifeq ($(CORE),ipu1_0)
 LNKCMD_FILE = $(PDK_CSL_COMP_PATH)/example/lnk_m4.cmd
 ifeq ($(SOC), $(filter $(SOC), am571x am572x))
  SRCS_COMMON = main_ipu.c ecc_ocmc.c
 else
  SRCS_COMMON = main_ipu.c ecc_emif.c ecc_ocmc.c
 endif
endif
ifeq ($(CORE),c66x)
 LNKCMD_FILE = $(PDK_CSL_COMP_PATH)/example/ecc/ecc_test_app/lnk_dsp.cmd
 ifeq ($(SOC), $(filter $(SOC), am571x am572x))
  SRCS_COMMON = main_c66x.c  ecc_ocmc.c
 else ifeq ($(SOC), $(filter $(SOC), k2g k2e k2k k2h k2l c6657 c6678))
  SRCS_COMMON = interrupt.c dsp_ecc.c dsp_icfg.c main_c66x.c
  SRCS_ASM_COMMON = intvecs.asm
  ifeq ($(SOC), $(filter $(SOC), k2g))
   SRCS_COMMON += ecc_emif_k2g.c
  endif
 else
  SRCS_COMMON = main_c66x.c  ecc_emif.c ecc_ocmc.c
 endif
endif
ifeq ($(CORE),a15_0)
 LNKCMD_FILE = $(PDK_CSL_COMP_PATH)/example/lnk_a15.cmd
 ifeq ($(SOC), $(filter $(SOC), am571x am572x))
  SRCS_COMMON = main.c ecc_ocmc.c
 else ifeq ($(SOC), $(filter $(SOC), k2g))
  SRCS_COMMON = main.c ecc_emif_k2g.c
 else
  SRCS_COMMON = main.c  ecc_emif.c ecc_ocmc.c
 endif
endif

ifeq ($(CORE),mpu1_0)
  SRCS_COMMON = main_a53.c ecc_msmc.c esm_app.c
endif

ifeq ($(SOC), $(filter $(SOC), j721e j7200 am64x j721s2 j784s4 j742s2))
ifeq ($(CORE),mcu1_0)
 EXTERNAL_LNKCMD_FILE_LOCAL = $(PDK_CSL_COMP_PATH)/example/ecc/ecc_test_app/linker_r5_$(SOC).lds
 SRCS_COMMON = main_r5f.c ecc_ddr.c esm_app_r5.c
ifeq ($(SOC), $(filter $(SOC), j721e))
 SRCS_COMMON += ecc_msmc_mem_parity.c
 CFLAGS_LOCAL_COMMON += -DMSMC_PARITY_TEST_SUPPORT
endif
endif
endif

LNKFLAGS_LOCAL_a15_0 += --entry Entry
LNKFLAGS_LOCAL_mpu1_0 += --entry Entry

ifeq ($(SOC),$(filter $(SOC), am65xx j721e j7200 am64x j721s2 j784s4 j742s2))
 SRCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/Unity/src
 SRCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/config
 INCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/Unity/src
 INCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/config
 SRCS_COMMON += unity_config.c unity.c
 CFLAGS_LOCAL_COMMON += -DUNITY_INCLUDE_CONFIG_H
endif

#If running without SBL, enable NO_SBL flag for DDR initialization with ECC
#CFLAGS_LOCAL_COMMON += -DNO_SBL

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
