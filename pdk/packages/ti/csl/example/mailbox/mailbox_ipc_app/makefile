#
# This file is the makefile for building CSL mailbox app.
#
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

APP_NAME = csl_mailbox_ipc_app
BUILD_OS_TYPE = baremetal
SRCDIR = .
INCDIR =

SRCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/Unity/src $(PDK_INSTALL_PATH)/ti/build/unit-test/config
INCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/Unity/src $(PDK_INSTALL_PATH)/ti/build/unit-test/config

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

COMP_LIST_COMMON += csl_utils_common csl_uart_console
ifeq ($(BOARD),$(filter $(BOARD), am65xx_evm am64x_evm))
  COMP_LIST_COMMON += $(PDK_COMMON_BAREMETAL_COMP)
else
  COMP_LIST_COMMON += csl 
  COMP_LIST_a15_0 = csl_init
endif

ifeq ($(SOC),$(filter $(SOC), am65xx am64x))
SRCS_COMMON = mailbox_ipc_main.c mailbox_ipc_app.c ipc_config.c
SRCDIR += ./$(SOC)
INCDIR += ./$(SOC)
endif

ifeq ($(SOC),$(filter $(SOC), am65xx am64x))
EXTERNAL_LNKCMD_FILE_LOCAL = $(SOC)/linker_$(CORE).lds
endif

PACKAGE_SRCS_COMMON = .
CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)

ifeq ($(ISA),$(filter $(ISA), a53 a72))
  LNKFLAGS_LOCAL_$(CORE) += --entry Entry
endif

# Baremetal Interrupt mode
CFLAGS_LOCAL_COMMON += -DBARE_METAL -DVIM_DIRECT_INTERRUPT_REGISTRATION -DMEASURE_LATENCY_DISTRIBUTION

# Baremetal Interrupt mode with detailed profiling
#CFLAGS_LOCAL_COMMON += -DBARE_METAL -DVIM_DIRECT_INTERRUPT_REGISTRATION -DMEASURE_LATENCY_DISTRIBUTION -DDETAILED_PROFILING

ifeq ($(CORE),mpu1_0)
SRCS_COMMON += unity.c unity_config.c
CFLAGS_LOCAL_COMMON += -DSYSTEM_MASTER -DUNITY_INCLUDE_CONFIG_H
endif

LNKFLAGS_LOCAL_a15host += --entry Entry

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
