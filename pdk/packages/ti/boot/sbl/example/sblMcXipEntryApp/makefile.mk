#
# This file is common makefile for building sbl_boot_xip_entry baremetal application
#

ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

APP_NAME = sbl_boot_multicore_xip_entry

BUILD_OS_TYPE = baremetal

SRCDIR += $(PDK_SBL_COMP_PATH)/example/sblMcXipEntryApp
INCDIR += $(PDK_SBL_COMP_PATH)/example/sblMcXipEntryApp

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)

SRCS_COMMON = main_baremetal.c

ifeq ($(CORE), mcu1_0)
  BUILD_MCU1_0=1
  EXTERNAL_LNKCMD_FILE_LOCAL = $(PDK_SBL_COMP_PATH)/example/sblMcXipEntryApp/$(SOC)/mcu1_0_entry.lds
  CFLAGS_LOCAL_COMMON += -DBUILD_MCU1_0
  COMP_LIST_COMMON = 
  INCLUDE_EXTERNAL_INTERFACES =
else
  BUILD_MCU2_0=1
  COMP_LIST_COMMON = $(PDK_COMMON_BAREMETAL_COMP)
  CFLAGS_LOCAL_COMMON += -DBUILD_MCU2_0
  INCLUDE_EXTERNAL_INTERFACES = pdk
  SRCS_COMMON += r5_config.c
endif

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON = .


CFLAGS_LOCAL_<core/SoC/platform-name> =

# Include common make files
ifeq ($(MAKERULEDIR), )
#Makerule path not defined, define this and assume relative path from ROOTDIR
  MAKERULEDIR := $(ROOTDIR)/ti/build/makerules
  export MAKERULEDIR
endif
include $(MAKERULEDIR)/common.mk
