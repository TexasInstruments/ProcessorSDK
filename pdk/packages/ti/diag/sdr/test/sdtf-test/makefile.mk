#
# This file is the makefile for building Software Diagnostics Reference test app
#
SRCDIR = src \
		 src/$(BOARD) \
		 src/$(BOARD)/r5f \
		 src/$(BOARD)/r5f/baremetal \
		 src/r5f/
INCDIR = ../../ src \
		 src/$(BOARD) \
		 src/$(BOARD)/r5f \
		 src/r5f/

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

# List all the components required by the application
COMP_LIST_COMMON = $(PDK_COMMON_BAREMETAL_COMP)
COMP_LIST_COMMON += sdr

SRCS_COMMON = sdtf_main.c sdtf_test.c sdtf_init.c sdtf_profile.c sdtf_injecterr.c \
			  sdtf_registercheck.c sdtf_wdt.c sdtf_esm.c sdtf_ecc.c sdtf_ccm.c \
			  sdtf_crc.c sdtf_mpu.c
SRCS_COMMON += sdtf_platform.c \
			   sdtf_core.c \
			   sdtf_exception.c \
			   sdtf_mpu_default.c \
			   sdtf_rat.c \
                           sdtf_utils.c

SRCS_ASM_COMMON += sdtf_utils_asm.asm
ifeq ($(ISA),$(filter $(ISA), a53, a72))
  LNKFLAGS_LOCAL_$(CORE) += --entry Entry
endif

EXTERNAL_LNKCMD_FILE_LOCAL = $(PDK_INSTALL_PATH)/ti/diag/sdr/test/sdtf-test/build/r5f/$(SOC)/linker_r5.lds

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON = . ../ ../../
SRCS_COMMON +=

CFLAGS_LOCAL_COMMON += $(PDK_CFLAGS)
CFLAGS_LOCAL_COMMON += -DUART_PRINTF

ifeq ($(SOC),$(filter $(SOC), am65xx j721e))
 SRCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/Unity/src
 SRCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/config
 INCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/Unity/src
 INCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/config
 SRCS_COMMON += unity_config.c unity.c
 CFLAGS_LOCAL_COMMON += -DUNITY_INCLUDE_CONFIG_H
 CFLAGS_LOCAL_COMMON += -DSDTF_BOARD
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
